#ifndef __CPPGEN_GNET_SKILL521
#define __CPPGEN_GNET_SKILL521
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill521:public Skill
    {
      public:
        enum
        { SKILL_ID = 521 };
          Skill521 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill521Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (0 + 0.15 * skill->GetT0 ());
                skill->SetPlus ((0.8 + 0.1 * skill->GetLevel ()) * skill->GetPlayer ()->GetMp ());
                skill->SetRatio (0 + 0.15 * skill->GetT0 ());
                skill->SetPlus ((0.8 + 0.1 * skill->GetLevel ()) * skill->GetPlayer ()->GetMp ());
                skill->GetPlayer ()->SetMp (0.5 * skill->GetPlayer ()->GetMp ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill521Stub ():SkillStub (521)
        {
            occupation = 13;
            name = L"¾ÛÁéÖä";
            nativename = "¾ÛÁéÖä";
            icon = "¾ÛÁéÖä.dds";
            maxlevel = 13;
            maxlearn = 7;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 0;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 650;
            talent[1] = 659;
            talent_size = 2;
            action[0] = "¹íÍõ×Ú_¾ÛÁéÖä";
            action[1] = "¹íÍõ×Ú_¾ÛÁéÖä";
            action[2] = "¹íÍõ×Ú_¾ÛÁéÖä";
            action[3] = "¹íÍõ×Ú_¾ÛÁéÖä";
            action[4] = "¹íÍõ×Ú_¾ÛÁéÖä";
            action[5] = "¹íÍõ×Ú_¾ÛÁéÖä";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_¾ÛÁéÖä";
            action[16] = "¹íÍõ×Ú_Æï³Ë_¾ÛÁéÖä";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¾ÛÁéÖä";
            action[18] = "¹íÍõ×Ú_Æï³Ë_¾ÛÁéÖä";
            action[19] = "¹íÍõ×Ú_Æï³Ë_¾ÛÁéÖä";
            action[20] = "¹íÍõ×Ú_Æï³Ë_¾ÛÁéÖä";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = false;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill521Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 519 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 75000 - 10000 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[7] = { 106, 109, 113, 117, 121, 125, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 80 + 10 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
#endif
    };
}
#endif
