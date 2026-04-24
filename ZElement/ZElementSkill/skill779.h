#ifndef __CPPGEN_GNET_SKILL779
#define __CPPGEN_GNET_SKILL779
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill779:public Skill
    {
      public:
        enum
        { SKILL_ID = 779 };
          Skill779 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill779Stub:public SkillStub
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
                skill->SetCrit (0.05);
                skill->SetRatio (1.33 + 0.05 * skill->GetT1 ());
                skill->SetPlus (35 * skill->GetT3 () * skill->GetLevel () + 980 + 50 * skill->GetLevel () +
                                2 * skill->GetLevel () * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill779Stub ():SkillStub (779)
        {
            occupation = 17;
            name = L"¾ÉÃÎ";
            nativename = "¾ÉÃÎ";
            icon = "¾ÉÃÎ.dds";
            maxlevel = 7;
            maxlearn = 6;
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
            skill_class = 1;
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
            talent[0] = 724;
            talent[1] = 732;
            talent[2] = 743;
            talent[3] = 765;
            talent_size = 4;
            action[0] = "ºÏ»¶ÅÉ_¾ÉÃÎ";
            action[1] = "ºÏ»¶ÅÉ_¾ÉÃÎ";
            action[2] = "ºÏ»¶ÅÉ_¾ÉÃÎ";
            action[3] = "ºÏ»¶ÅÉ_¾ÉÃÎ";
            action[4] = "ºÏ»¶ÅÉ_¾ÉÃÎ";
            action[5] = "ºÏ»¶ÅÉ_¾ÉÃÎ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_¾ÉÃÎ";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_¾ÉÃÎ";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_¾ÉÃÎ";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¾ÉÃÎ";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_¾ÉÃÎ";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_¾ÉÃÎ";
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
            doenchant = true;
            dobless = true;
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
        virtual ~ Skill779Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 0 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 24 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.1 * skill->GetLevel () * skill->GetLevel () + 1.7 * skill->GetLevel () + 360);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 0;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 135, 135, 135, 135, 136, 137 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (8);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.1 * skill->GetLevel () * skill->GetLevel () + 1.7 * skill->GetLevel () + 360),
                               5,
                               33,
                               INT (980 + 50 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ()), 10 + 3 * skill->GetLevel (), 2, 7);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * ((60 + 8 * skill->GetT2 ()) * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetRatio (0.48 * (INT (zrand (100) / (100 - 4 * skill->GetT2 ())) + 1));
            skill->GetVictim ()->SetDrainmagic (1);
            skill->GetVictim ()->SetProbability (1.0 * ((10 + 3 * skill->GetLevel () + 5 * skill->GetT3 ())));
            skill->GetVictim ()->SetAmount (2 + int (skill->GetLevel () / 6.5));
              skill->GetVictim ()->SetClearbuff (1);
              return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (8 * skill->GetT0 ()));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (0.02 * skill->GetT0 () * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHeal (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (293);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (2000);
            skill->GetVictim ()->SetReduceskillcd (1);
            return true;
        }
#endif
    };
}
#endif
