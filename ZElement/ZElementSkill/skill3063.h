#ifndef __CPPGEN_GNET_SKILL3063
#define __CPPGEN_GNET_SKILL3063
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3063:public Skill
    {
      public:
        enum
        { SKILL_ID = 3063 };
          Skill3063 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3063Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3063Stub ():SkillStub (3063)
        {
            occupation = 168;
            name = L"ø’¡È<…∑>";
            nativename = "ø’¡È<…∑>";
            icon = "ƒß°§ø’¡È.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 2;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
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
            action[0] = "«‡‘∆√≈_ø’¡È";
            action[1] = "«‡‘∆√≈_ø’¡È";
            action[2] = "πÌÕı◊⁄_ø’¡È";
            action[3] = "∫œª∂≈…_ø’¡È";
            action[4] = "ÃÏ“ÙÀ¬_ø’¡È";
            action[5] = "πÌµ¿_ø’¡È";
            action[6] = "∑Ÿœ„π»_ø’¡È";
            action[7] = " ﬁ…Ò_ø’¡È";
            action[8] = "∫¸—˝_ø’¡È";
            action[9] = "–˘‘Ø_ø’¡È";
            action[10] = "ª≥π‚_ø’¡È";
            action[11] = "Ã´Íª_ø’¡È";
            action[12] = "ÃÏª™_ø’¡È";
            action[13] = "¡ÈŸÌ_ø’¡È";
            action[14] = "”¢’–_ø’¡È";
            action[15] = "«‡‘∆√≈_∆Ô≥À_ø’¡È";
            action[16] = "«‡‘∆√≈_∆Ô≥À_ø’¡È";
            action[17] = "πÌÕı◊⁄_∆Ô≥À_ø’¡È";
            action[18] = "∫œª∂≈…_∆Ô≥À_ø’¡È";
            action[19] = "ÃÏ“ÙÀ¬_∆Ô≥À_ø’¡È";
            action[20] = "πÌµ¿_∆Ô≥À_ø’¡È";
            action[21] = "∑Ÿœ„π»_∆Ô≥À_ø’¡È";
            action[22] = " ﬁ…Ò_∆Ô≥À_ø’¡È";
            action[23] = "∫¸—˝_∆Ô≥À_ø’¡È";
            action[24] = "–˘‘Ø_∆Ô≥À_ø’¡È";
            action[25] = "ª≥π‚_∆Ô≥À_ø’¡È";
            action[26] = "Ã´Íª_∆Ô≥À_ø’¡È";
            action[27] = "ÃÏª™_∆Ô≥À_ø’¡È";
            action[28] = "¡ÈŸÌ_∆Ô≥À_ø’¡È";
            action[29] = "”¢’–_∆Ô≥À_ø’¡È";
            action[30] = "0";
            rangetype = 5;
            doenchant = true;
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
        virtual ~ Skill3063Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1549 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 200, 500, 500, 600, 900, 1900, 3200, 4500, 8600, 10800, 20700, 32200, 51000, 65200, 84800, 159900, 260100, 427500, 743600,
 1450600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 11, 14, 17, 21, 25, 29, 34, 39, 44, 49, 55, 61, 67, 73, 79 };
            return array[level - 1];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (325 + 25 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 720000 - 30000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               325 + 25 * skill->GetLevel (),
                               180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               10 + skill->GetLevel (),
                               720 - 30 * skill->GetLevel (), skill->GetLevel (), 12 * skill->GetLevel (), 10 + skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (10000 + 1000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (10 + skill->GetLevel ());
            skill->GetVictim ()->SetValue (12 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 12 * skill->GetLevel ());
            skill->GetVictim ()->SetFrenzied (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (10000 + 1000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetRatio (0.0101 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncskilldodge (1);
            return true;
        }
#endif
    };
}
#endif
